import React, { Component } from "react";
import {Link} from 'react-router-dom'
import PropTypes from 'prop-types';

/**
 * Application sidebar menu.
 * 
 * @author Diogo S. Mendonça
 * @example
 * var layout = React.createRef();
 * return (
 *   <div id="layout" ref={layout}>
 *      <Menu layout={layout} />
 *   </div>
 * )
 *  
 */
class Menu extends Component {

    /**
     * @param {Object} props {layout: element} the element that suround the menu.
     */
    constructor(props){
        super(props);
        this._menu = React.createRef();
        this._menuLink = React.createRef();
        this.state = {
            active: false //whether the menu sidebar is displayed
        };
    }

    /**
     * Hides the menu sidebar. Closes the menu if it is opened 
     * in small screen sizes.
     */
    hideMenu(){
        if (this.state.active) {
            this.toggleMenu();
        }
    }
    
    /**
     * Toggle a CSS class in a DOMElement. Include the class if 
     * it is not present. If the class is present in the DOMElement,
     * removes it.
     * 
     * @param {DOMElement} element element to have the CSS class toggled
     * @param {String} className CSS class to be toggled
     */
    _toggleClass(element, className) {
        var classes = element.className.split(/\s+/),
            length = classes.length,
            i = 0;

        for(; i < length; i++) {
          if (classes[i] === className) {
            classes.splice(i, 1);
            break;
          }
        }
        // The className is not found
        if (length === classes.length) {
            classes.push(className);
        }

        element.className = classes.join(' ');
    }

    /**
     * Change the state of the menu by toggling the active CSS class of all DOMElements 
     * needed to show or hide the sidebar menu in small screens.
     * In case, the menu and menuLink DOMElements stored as references
     * to the nodes, and layout element, which the reference is passed as props.
     */
    toggleMenu() {
        var active = 'active';
        this.setState({active: !this.state.active});
        //e.preventDefault();
        this._toggleClass(this.props.layout.current, active);
        this._toggleClass(this._menu.current, active);
        this._toggleClass(this._menuLink.current, active);
    }

    /**
     * Presents the menu sidebar.
     * Include Link and onClick event handlers to toggle the menu sidebar and to access
     *  the functionalities of the system.
     */
    render(){
        return(
            <>
                <a href="#menu" ref={this._menuLink} id="menuLink" className="menu-link" onClick={(e) => this.toggleMenu(e)}>
                    <span></span>
                </a>

                <div id="menu" ref={this._menu}>
                    <div className="pure-menu">
                        <a className="pure-menu-heading" href="/">DoorLocking.App</a>
                        <ul className="pure-menu-list">
                            <li className="pure-menu-item"><Link onClick={(e) => this.hideMenu(e)} to='/open_door' className="pure-menu-link">Open Door</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.hideMenu(e)} to='/register_user' className="pure-menu-link">New User</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.hideMenu(e)} to='/list_users' className="pure-menu-link">List/Change Users</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.hideMenu(e)} to='/config' className="pure-menu-link">Configuration</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.hideMenu(e)} to='/logout_handler' className="pure-menu-link">Logout</Link></li>
                        </ul>
                    </div>
                </div>
            </>
        );
    }
}

Menu.propTypes = {
    /**
     * layout is the element that suround the menu.
     */
    layout: PropTypes.element.isRequired,
}

export default Menu;