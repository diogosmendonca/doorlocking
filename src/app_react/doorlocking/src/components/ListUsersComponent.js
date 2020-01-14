import React from "react";
import {Redirect} from 'react-router-dom';

export default function ListUsers(props) {
    return(
        <>
            <div className="header">
                <h1>List of User</h1>
                <h2>Click to change user status</h2>
                {/* eslint-disable-next-line*/}
                <h4 id="msg"></h4>
            </div>

            <div className="content">
                <br/>
                <table className="pure-table">
                    <thead>
                        <tr valign="middle">
                            <th>Username</th>
                            <th>Status/Change</th>
                        </tr>
                    </thead>
                    <tbody id="users">
                    </tbody>
                </table>
                <br/>
                <button type='button' className="pure-button pure-button-secondary" onClick={()=> props.history.push("/menu")}>Back</button>
                <br/><br/>
            </div>
        </>
    );
}